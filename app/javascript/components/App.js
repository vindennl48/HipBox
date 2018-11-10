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
      currentUser: null,
      name: 'setValue',
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
        <div>
          <MuteButton variable="mitch_james_mute" />
          <SoloButton variable="mitch_mitch_solo" />
          <SoloButton />
          <button onClick={()=>{ this.setValue(84) }}>{this.state.name}</button>
          <ValueDisplay
            setValue={(func)=>{ this.setValue = func }}
            callback={(value)=>{ this.setState({ name: value }) }}
          />
          <div className="test-wrap">
            <ChanStrip />
          </div>
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
