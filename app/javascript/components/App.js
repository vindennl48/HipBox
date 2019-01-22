import React from "react"
import PropTypes from "prop-types"

import ChanStrip from "./ChanStrips/ChanStrip"
import ChanStripSys from "./ChanStrips/ChanStripSys"
import ChanStripExtra from "./ChanStrips/ChanStripExtra"


class App extends React.Component {
  constructor(props) {
    super(props)

    this.state = {
      currentUser: null,
    }
  }

  componentDidMount() {
    fetch("/api/v1/users")
      .then((response) => { return response.json() })
      .then((user) => {
        this.setState({ currentUser: user })
      })
  }

  setValue() { console.log('not working') }

  render () {
    const { currentUser } = this.state

    if (currentUser) {

      return (
        <div className="bo-wrapper">
          <ChanStrip    user={currentUser.name} chan="james" />
          <ChanStrip    user={currentUser.name} chan="jesse" />
          <ChanStrip    user={currentUser.name} chan="mitch" />
          <ChanStrip    user={currentUser.name} chan="drums" />
          <ChanStripSys user={currentUser.name} chan="hp"       label="HPVol" />
          <ChanStripSys user={currentUser.name} chan="click"    label="Click" />
          <ChanStripSys user={currentUser.name} chan="talkback" label="Tlkbk" />
          <ChanStripExtra />
        </div>
      )

    } else {
      return (
        <div className="bo-loading">
          <div>Loading..... <button onClick={()=> location.reload(true)}>RESET</button></div>
        </div>
      )
    }
  }
}

export default App
