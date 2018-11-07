import React from "react"
import PropTypes from "prop-types"
import ToggleButton from "./ToggleButton"


class HPRerouteButton extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      hpr: null,
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
          .then((hpr)     => {
            this.setValue(hpr.status)
            this.setState({ hpr: {
              id:     hpr.id,
              status: hpr.status,
            }})
          })
        setTimeout(pollServer, 1000)
      }
      setTimeout(pollServer, 1000)
    }
  }

  save = (value) => {
    let { hpr } = this.state

    if (hpr) {
      hpr.status = value

      this.setState({ hpr: hpr })

      fetch(`/api/v1/variables/${hpr.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(hpr),
      })
        .then((response) => { return response.json() })
        //.then((hpr)     => { console.log(hpr) })
    }
  }

  setValue() { /* call-forward for ToggleButton.setValue */ }

  render () {
    return (
      <ToggleButton
        {...this.props}
        label           = "HPR"
        bgColorActive   = "#fff600"
        textColorActive = "#000"
        callback        = {this.save}
        setValue        = {(func) => { this.setValue = func }}
        width           = "50px"
        height          = "50px"
        mediaQuery      = ""
      />
    );
  }
}

HPRerouteButton.defaultProps = {
  variable: null,
}

export default HPRerouteButton
