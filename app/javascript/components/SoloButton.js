import React from "react"
import PropTypes from "prop-types"
import ToggleButton from "./ToggleButton"


class SoloButton extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      solo:      null,
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
          .then((solo)     => {
            if (!this.state.pollBlock) {
              this.setValue(solo.status)
              this.setState({ solo: {
                id:     solo.id,
                status: solo.status,
              }})
            } else {
              this.setState({ pollBlock: false })
            }
          })
        setTimeout(pollServer, 1000)
      }
      pollServer()
    }
  }

  save = (value) => {
    let { solo } = this.state

    if (solo) {
      solo.status = value

      this.setState({
        solo:      solo,
        pollBlock: true,
      })

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
