import React from "react"
import PropTypes from "prop-types"
import ToggleButton from "./ToggleButton"


class MuteButton extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      mute: null,
    }
  }

  componentDidMount() {
    const { variable } = this.props

    fetch("/api/v1/variables", {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({ name: variable })
    })
      .then((response) => { return response.json() })
      .then((mute)     => {
        this.setValue(mute.status)
        this.setState({ mute: mute })
      })
  }

  save = (value) => {

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

export default MuteButton
