import React from "react"
import PropTypes from "prop-types"


const ToggleAjax = (WrappedComponent) => {
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

    componentDidMount() {
      const { variable } = this.props
      if (variable) {
        let cable = ActionCable.createConsumer('/cable')

        this.osc_data = cable.subscriptions.create({
          channel: "OscDataChannel",
          room: variable
        }, {
          connected: () => {
            console.log(`ActionCable: connection established for "${variable}"`)
            this.osc_data.askForData()
            //this.pollServer()
          },
          disconnected: () => {
            console.log(`ActionCable: connection disconnected for "${variable}"`)
          },
          received: (data) => {
            const record = data.record
            this.setValue(record.status)
            this.setState({ record: {
              id:    record.id,
              status: record.status,
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
        record.status = value
        this.setState({
          record:    record,
        })
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

export default ToggleAjax
