import React from "react"
import PropTypes from "prop-types"

import ChannelStrip from "./ChannelStrip"


class TalkbackVolume extends React.Component {
  constructor (props) {
    super(props)
    this.state = {
      user: null,
      mute: null,
    }
  }

  componentDidMount() {
    fetch("/api/v1/users")
      .then((response) => { return response.json() })
      .then((user)     => {
        this.setVolume(user.tbVol)
        this.setState({
          user: {
            id:    user.id,
            tbVol: user.tbVol,
          }
        })
      })

    fetch("/api/v1/variables", {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({name: 'talkback_toggle'})
    })
      .then((response) => { return response.json() })
      .then((mute)     => {
        this.setMute(mute.status)
        this.setState({ mute: mute })
      })
  }

  volumeChange = (value) => {
    let { user } = this.state

    if (user) {
      user.tbVol = value
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
        //.then((user)     => { console.log('volume: ', user.tbVol) })
    }
  }

  muteChange = (value) => {
    let { mute } = this.state

    if (mute) {
      mute.status = value

      //console.log("mute_value: ", value)

      this.setState({mute: mute})

      fetch(`/api/v1/variables/${mute.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(mute),
      })
        .then((response) => { return response.json() })
        //.then((data)     => { console.log(data) })
    }
  }

  render () {
    return (
      <ChannelStrip
        label          = "Tlkbk Vol"
        soloIsDisabled = {true}
        muteIsDisabled = {false}
        soloRemote     = {false}
        onChange       = {this.volumeChange}
        onMuteChange   = {this.muteChange}
        setValue       = {(func) => { this.setVolume = func }}
        setMute        = {(func) => { this.setMute = func }}
      />
    )
  }
}

export default TalkbackVolume
