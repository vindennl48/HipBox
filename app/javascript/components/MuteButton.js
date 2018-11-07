import React from "react"
import PropTypes from "prop-types"
import ToggleButton from "./ToggleButton"


class MuteButton extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      mute:      null,
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
          .then((mute)     => {
            if (!this.state.pollBlock) {
              this.setValue(mute.status)
              this.setState({ mute: {
                id:     mute.id,
                status: mute.status,
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
    let { mute } = this.state

    if (mute) {
      mute.status = value

      this.setState({
        mute:      mute,
        pollBlock: true,
      })

      fetch(`/api/v1/variables/${mute.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(mute),
      })
        .then((response) => { return response.json() })
        //.then((mute)     => { console.log(mute) })
    }
  }

  setValue() { /* call-forward for ToggleButton.setValue */ }

  render () {
    return (
      <ToggleButton
        {...this.props}
        label="Mute"
        bgColorActive="#ff9393"
        callback={this.save}
        setValue={(func) => { this.setValue = func }}
      />
    );
  }
}

MuteButton.defaultProps = {
  variable: null,
}

export default MuteButton
