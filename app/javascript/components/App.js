import React from "react"
import PropTypes from "prop-types"

import MuteButton from "./Buttons/MuteButton"
import SoloButton from "./Buttons/SoloButton"
import ValueDisplay from "./Displays/ValueDisplay"
import ChanStrip from "./ChanStrips/ChanStrip"

class App extends React.Component {
  constructor(props) {
    super(props)

    this.state = {
      currentUser: null
    }
  }

  componentDidMount() {
    fetch("/api/v1/users")
      .then((response) => { return response.json() })
      .then((user) => {
        this.setState({ currentUser: user })
      })
  }

  render () {
    const { currentUser } = this.state

    if (currentUser) {

      return (
        <div>
          <MuteButton variable="mitch_james_mute" />
          <SoloButton variable="mitch_mitch_solo" />
          <SoloButton />
          <ValueDisplay />
          <ChanStrip />
        </div>
      )

    } else {
      return (
        <div>loading..</div>
      )
    }
  }
}

export default App
