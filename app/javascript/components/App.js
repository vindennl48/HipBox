import React from "react"
import PropTypes from "prop-types"

import ChanStrip from "./ChanStrip"
import ChanStripExtra from "./ChanStripExtra"


class App extends React.Component {
  constructor(props) {
    super(props)

    this.state = {
      user:       null,
      chanStrips: {},
      order:      [],
    }
  }

  componentDidMount() {
    const { user, variables } = this.props
    let { chanStrips, order } = this.state

    variables.map((variable)=>{

      if (!(variable["name"] in chanStrips)) {
        chanStrips[variable["name"]] = {}
      }

      chanStrips[variable["name"]][variable["kind"]] = variable

      if (!(order.includes(variable["name"]))) {
        order.push(variable["name"])
      }

    })

    this.setState({
      user:       user,
      chanStrips: chanStrips
    })

    let cable = ActionCable.createConsumer('/cable')

    this.userChannel = cable.subscriptions.create({
      channel: "UserChannel",
      room: user
    }, {
      connected: () => {
        console.log(`ActionCable: connection established for "${user.name}"`)
      },
      disconnected: () => {
        console.log(`ActionCable: connection disconnected for "${user.name}"`)
        location.reload(true)
      },
      received: (data) => {
        const records     = data.records
        let { chanStrips } = this.state

        records.map((record)=>{
          chanStrips[record["name"]][record["kind"]] = record
        })

        this.setState({ chanStrips: chanStrips })

      },
      askForData: () => {
        this.userChannel.perform('askForData')
      },
      sendData: (record) => {
        this.userChannel.perform('sendData', { record: record })
      },
    })
  }

  setVariable = (name, kind, value) => {
    let { chanStrips } = this.state
    chanStrips[name][kind]["value"] = value

    if (kind == "vol") {
      this.setState({ chanStrips: chanStrips })
    }

    this.userChannel.sendData(chanStrips[name][kind])
  }

  render () {
    const { user, chanStrips, order } = this.state

    if (user != null) {

      return (
        <div className="bo-wrapper">
          {
            order.map((name, i)=>{
              let data = chanStrips[name]
              if ("vol" in data) {
                return (
                  <ChanStrip
                    key      = {i}
                    user     = {user["name"]}
                    chan     = {name}
                    volu     = {data["vol"]}
                    mute     = {data["mute"] || null}
                    solo     = {data["solo"] || null}
                    callback = {this.setVariable}
                  />
                )
              } else if ("record" in data) {
                return (
                  <ChanStripExtra
                    key      = {i}
                    chan     = {name}
                    rec      = {data["record"] || null}
                    callback = {this.setVariable}
                  />
                )
              }
            })
          }
        </div>
      )

    } else {
      return (
        <div className="bo-loading">Error loading user..</div>
      )
    }
  }
}

export default App
