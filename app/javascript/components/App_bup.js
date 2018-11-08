import React from "react"
import PropTypes from "prop-types"

import HPVolumeChanStrip from "./HPVolumeChanStrip"
import ClickVolumeChanStrip from "./ClickVolumeChanStrip"
import TalkbackVolumeChanStrip from "./TalkbackVolumeChanStrip"
import InstVolumeChanStrip from "./InstVolumeChanStrip"
import ExtraChanStrip from "./ExtraChanStrip"
import BassEffectsChanStrip from "./BassEffectsChanStrip"


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
    let mitch = null

    if (currentUser) {

      if (currentUser.name == 'mitch') {
        mitch = <BassEffectsChanStrip />
      }

      return (
        <div className="body wrapper">
          <InstVolumeChanStrip     currentUser={currentUser.name} member="james" />
          <InstVolumeChanStrip     currentUser={currentUser.name} member="jesse" />
          <InstVolumeChanStrip     currentUser={currentUser.name} member="mitch" />
          <InstVolumeChanStrip     currentUser={currentUser.name} member="drums" />
          <HPVolumeChanStrip       currentUser={currentUser.name} />
          <ClickVolumeChanStrip    currentUser={currentUser.name} />
          <TalkbackVolumeChanStrip currentUser={currentUser.name} />
          <ExtraChanStrip          currentUser={currentUser.name} />

          {mitch}

        </div>
      )
    } else {
      return (
        <div className="body loading">loading...</div>
      )
    }
  }
}

export default App
