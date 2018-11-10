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

    componentDidMount() {
      const { variable, callback } = this.props

      this.props.setValue((value) => { this.setValue(value) })

      if (variable) {

        let pollServer = () => {
          const { record } = this.state
          fetch(`/api/v1/variables/${record.id}`, {
            method: 'PUT',
            headers: {
              'Content-Type': 'application/json'
            },
            body: JSON.stringify(record),
          })
            .then((response) => { return response.json() })
            //.then((record)     => { console.log(record) })
          setTimeout(pollServer, 1000)
        }

        fetch("/api/v1/variables", {
          method: 'PUT',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({ name: variable })
        })
          .then((response) => { return response.json() })
          .then((record)     => {
            callback(record.value)
            this.setValue(record.value)
            this.setState({ record: {
              id:    record.id,
              value: record.value,
            }})
            pollServer()
          })
          .catch((error) => { alert(`Error: "${error}"`);location.reload() })

      }
    }

    save = (value) => {
      let { record } = this.state

      this.props.callback(value)

      if (record) {
        record.value = value

        this.setState({ record: record })

        fetch(`/api/v1/variables/${record.id}`, {
          method: 'PUT',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify(record),
        })
          .then((response) => { return response.json() })
          //.then((record)     => { console.log(record) })
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
