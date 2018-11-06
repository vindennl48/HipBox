import React from "react"
import PropTypes from "prop-types"
import styles from "./ChannelStripStyles"

import VolumeSlider from "./VolumeSlider"
import SoloButton from "./SoloButton"
import MuteButton from "./MuteButton"


class InstrumentVolume extends React.Component {
  constructor(props) {
    super(props)
    this.state = { volume: 0 }
  }

  capitalize(word) {
    return word.charAt(0).toUpperCase()+word.slice(1)
  }

  render () {
    const { currentUser, member } = this.props
    const { volume } = this.state
    return (
      <div className={styles.wrapper}>

        <div className={styles.border}>

          <div className={styles.information}>
            <div>{this.capitalize(member)}</div>
          </div>

          <div className={styles.body}>

            <div className={styles.slider}>
              <VolumeSlider
                variable           = {`${currentUser}_${member}_vol`}
                sendValueToParent  = {(value) => { this.setState({ volume: value }) }}
                getValueFromParent = {(func)  => { this.setVolume = func }}
              />
            </div>

            <div className={styles.buttons}>
              <div onClick={() => { this.setVolume(84) }}>
                {`${volume == 84 ? '' : '*'}${volume}`}
              </div>

              <SoloButton
                isDisabled = {currentUser == member ? false : true}
                variable   = {`${member}_${member}_solo`}
              />

              <MuteButton
                variable = {`${currentUser}_${member}_mute`}
              />

            </div>

          </div>

        </div>

      </div>
    )
  }
}

export default InstrumentVolume
