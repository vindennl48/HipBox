import React from "react"
import PropTypes from "prop-types"
import styles from "./AppStyles"

//import HeadphoneVolume from "./HeadphoneVolume"
//import ClickVolume from "./ClickVolume"
//import TalkbackVolume from "./TalkbackVolume"
import ToggleButton from "./ToggleButtonRaw"


class App extends React.Component {
  render () {
    return (
      <ToggleButton label="Mute" />
    )
    //return (
      //<div className={styles.wrapper}>
        //<HeadphoneVolume />
        //<ClickVolume />
        //<TalkbackVolume />
      //</div>
    //)

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
