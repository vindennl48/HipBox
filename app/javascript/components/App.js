import React from "react"
import PropTypes from "prop-types"
import OSC from "osc-js"


class App extends React.Component {
  constructor(props) {
    super(props)
  }

  componentDidMount() {
    const options = { send: { host: '192.168.2.7', port: 3001 } }
    const osc = new OSC({ plugin: new OSC.WebsocketClientPlugin(options) })
    //osc.send(new OSC.Message('/mitch_mitch_vol'))
  }

  render () {
    return (
      <div>hey</div>
    )
  }
}

export default App
