import React from "react"
import PropTypes from "prop-types"

import ChannelStrip from "./ChannelStrip"


class ClickVolume extends React.Component {
  constructor (props) {
    super(props)
    this.state = {
      user: null
    }
  }

  componentDidMount() {
    fetch("/api/v1/users")
      .then((response) => { return response.json() })
      .then((user)     => {
        this.setValue(user.clickVol)
        this.setState({
          user: {
            id:       user.id,
            clickVol: user.clickVol,
          }
        })
      })
  }

  volumeChange = (value) => {
    let { user } = this.state

    if (user){
      user.clickVol = value
      this.setState({ user: user })

      //console.log("volume: ", value)

      fetch(`/api/v1/users/${user.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(user),
      })
        .then((response) => { return response.json() })
        //.then((data)     => { console.log(data) })
    }
  }

  render () {
    return (
      <ChannelStrip
        label          = "Click Vol"
        soloIsDisabled = {true}
        muteIsDisabled = {true}
        soloRemote     = {false}
        onChange       = {this.volumeChange}
        setValue       = {(func) => { this.setValue = func }}
      />
    )
  }
}

export default ClickVolume
