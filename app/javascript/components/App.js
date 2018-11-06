import React from "react"
import PropTypes from "prop-types"
import styles from "./AppStyles"

import HeadphoneVolume from "./HeadphoneVolume"
import ClickVolume from "./ClickVolume"
import TalkbackVolume from "./TalkbackVolume"
import InstrumentVolume from "./InstrumentVolume"
import ExtraChanStrip from "./ExtraChanStrip"


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
        <div className={styles.wrapper}>
          <InstrumentVolume currentUser={currentUser.name} member="james" />
          <InstrumentVolume currentUser={currentUser.name} member="jesse" />
          <InstrumentVolume currentUser={currentUser.name} member="mitch" />
          <InstrumentVolume currentUser={currentUser.name} member="drums" />
          <HeadphoneVolume  currentUser={currentUser.name} />
          <ClickVolume      currentUser={currentUser.name} />
          <TalkbackVolume   currentUser={currentUser.name} />
          <ExtraChanStrip   currentUser={currentUser.name} />
        </div>
      )
    } else {
      return (
        <div className={styles.loading_wrapper}>loading...</div>
      )
    }
  }
}

export default App




  //constructor (props) {
    //super(props)

    //this.timer = null

    //this.state = {
      //jamesSoloRemote: false,
      //jesseSoloRemote: false,
      //drumsSoloRemote: false,
    //}
  //}

  //componentDidMount () {
    //// uncomment when api is ready
    ////this.timer = setInterval(() => this.refreshData(), 2000)
  //}

  //componentWillUnmount () {
    //clearInterval(this.timer)
    //this.timer = null
  //}

  //refreshData () {
    //fetch('/api/v1/refresh.json')
      //.then((response) => { return response.json() })
      //.then((data)     => {
        //this.setState({
          //jamesSoloRemote: data.james.solo,
          //jesseSoloRemote: data.jesse.solo,
          //drumsSoloRemote: data.drums.solo,
        //})
      //})
  //}



    //const { jamesSoloRemote } = this.state
    //const { jesseSoloRemote } = this.state
    //const { drumsSoloRemote } = this.state
    //return (
      //<div className={styles.wrapper}>

        //<ChannelStrip label="James"
          //soloIsDisabled = {true}
          //soloRemote     = {jamesSoloRemote}
        ///>

        //<ChannelStrip label="Jesse"
          //soloIsDisabled = {true}
          //soloRemote     = {jesseSoloRemote}
        ///>

        //<ChannelStrip label="Mitch" />

        //<ChannelStrip label="Drums"
          //soloIsDisabled = {true}
          //soloRemote     = {drumsSoloRemote}
        ///>

        //<ChannelStrip label="HP Vol"
          //soloIsDisabled = {true}
          //muteIsDisabled = {true}
        ///>

        //<ChannelStrip label="Click"
          //soloIsDisabled = {true}
        ///>

        //<ChannelStrip label="Tlkbk"
          //soloIsDisabled = {true}
        ///>

      //</div>
    //)
