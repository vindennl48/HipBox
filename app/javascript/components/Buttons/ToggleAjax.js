import React from "react"
import PropTypes from "prop-types"


const ToggleAjax = (WrappedComponent) => {
  return class HOC extends React.Component {
    constructor (props) {
      super(props)

      this.state = {
        record:    null,
        pollBlock: false,
      }
    }

    componentDidMount() {
      const { variable } = this.props
      if (variable) {
        let pollServer = () => {
          fetch("/api/v1/variables", {
            method: 'PUT',
            headers: {
              'Content-Type': 'application/json'
            },
            body: JSON.stringify({ name: variable })
          })
            .then((response) => { return response.json() })
            .then((record)     => {
              if (!this.state.pollBlock) {
                this.setValue(record.status)
                this.setState({ record: {
                  id:     record.id,
                  status: record.status,
                }})
              } else {
                this.setState({ pollBlock: false })
              }
            })
            .catch(() => { location.reload() })
          setTimeout(pollServer, 1000)
        }
        pollServer()
      }
    }

    save = (value) => {
      let { record } = this.state

      if (record) {
        record.status = value

        this.setState({
          record:      record,
          pollBlock: true,
        })

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

export default ToggleAjax
