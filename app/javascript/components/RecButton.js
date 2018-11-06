import React from "react"
import PropTypes from "prop-types"
import ToggleButton from "./ToggleButton"


class RecButton extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      rec: null,
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
          .then((rec)     => {
            this.setValue(rec.status)
            this.setState({ rec: {
              id:     rec.id,
              status: rec.status,
            }})
          })
        setTimeout(pollServer, 1000)
      }
      setTimeout(pollServer, 1000)
    }
  }

  save = (value) => {
    let { rec } = this.state

    if (rec) {
      rec.status = value

      this.setState({ rec: rec })

      fetch(`/api/v1/variables/${rec.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(rec),
      })
        .then((response) => { return response.json() })
        //.then((rec)     => { console.log(rec) })
    }
  }

  setValue() { /* call-forward for ToggleButton.setValue */ }

  render () {
    return (
      <ToggleButton
        {...this.props}
        label           = "Rec"
        bgColorActive   = "#ff0000"
        textColorActive = "#fff"
        callback        = {this.save}
        setValue        = {(func) => { this.setValue = func }}
      />
    );
  }
}

RecButton.defaultProps = {
  variable: null,
}

export default RecButton