import React from "react"
import PropTypes from "prop-types"


const SliderAjax = (WrappedComponent) => {
  return class HOC extends React.Component {
    static defaultProps = {
      setValue: ()=>{},
      callback: ()=>{},
    }

    constructor (props) {
      super(props)

      this.state = {
        record: null,
      }
    }

    pollServer = () => {
      const { record } = this.state
      if (record != null) {
        this.osc_data.sendData(record)
      } else {
        this.osc_data.askForData()
      }
      setTimeout(this.pollServer, 1000)
    }

    componentDidMount() {
      const { variable, callback } = this.props

      this.props.setValue((value) => { this.setValue(value) })

      if (variable) {
        let cable = ActionCable.createConsumer('/cable')

        this.osc_data = cable.subscriptions.create({
          channel: "OscDataChannel",
          room: variable
        }, {
          connected: () => {
            console.log(`ActionCable: connection established for "${variable}"`)
            //this.osc_data.askForData()
            this.pollServer()
          },
          disconnected: () => {
            console.log(`ActionCable: connection disconnected for "${variable}"`)
            location.reload(true)
          },
          received: (data) => {
            const record = data.record
            callback(record.value)
            this.setValue(record.value)
            this.setState({ record: {
              id:    record.id,
              value: record.value,
            }})
          },
          askForData: () => {
            this.osc_data.perform('askForData')
          },
          sendData: (record) => {
            this.osc_data.perform('sendData', { record: record })
          },
        })

      }
    }

    save = (value) => {
      let { record } = this.state
      this.props.callback(value)

      if (record) {
        record.value = value
        this.setState({ record: record })
        this.osc_data.sendData(record)
      }
    }

    setValue() { /* call-forward for ToggleButton.setValue */ }

    render () {
      return (
        <WrappedComponent
          {...this.props}
          callback={this.save}
          setValue={(func) => { this.setValue = func }}
        />
      );
    }
  }
}

export default SliderAjax
