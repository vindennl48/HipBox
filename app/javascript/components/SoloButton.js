import React from "react"
import PropTypes from "prop-types"
import ToggleButton from "./ToggleButton"


class SoloButton extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      solo: null,
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
          .then((solo)     => {
            this.setValue(solo.status)
            this.setState({ solo: {
              id:     solo.id,
              status: solo.status,
            }})
          })
        setTimeout(pollServer, 1000)
      }
      setTimeout(pollServer, 1000)
    }
  }

  save = (value) => {
    let { solo } = this.state

    if (solo) {
      solo.status = value

      this.setState({ solo: solo })

      fetch(`/api/v1/variables/${solo.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(solo),
      })
        .then((response) => { return response.json() })
        //.then((solo)     => { console.log(solo) })
    }
  }

  setValue() { /* call-forward for ToggleButton.setValue */ }

  render () {
    return (
      <ToggleButton
        {...this.props}
        label="Solo"
        bgColorActive="#fffb93"
        callback={this.save}
        setValue={(func) => { this.setValue = func }}
      />
    );
  }
}

SoloButton.defaultProps = {
  variable: null,
}

export default SoloButton
