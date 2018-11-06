import React from "react"
import PropTypes from "prop-types"
import styles from "./ChannelStripStyles"

import VolumeSlider from "./VolumeSlider"
import SoloButton from "./SoloButton"
import MuteButton from "./MuteButton"


class ClickVolume extends React.Component {
  constructor(props) {
    super(props)
    this.state = { volume: 0 }
  }

  render () {
    const { volume } = this.state
    const { currentUser } = this.props
    return (
      <div className={styles.wrapper}>

        <div className={styles.border}>

          <div className={styles.information}>
            <div>Click</div>
          </div>

          <div className={styles.body}>

            <div className={styles.slider}>
              <VolumeSlider
                variable           = {`${currentUser}_click_vol`}
                sendValueToParent  = {(value) => { this.setState({ volume: value }) }}
                getValueFromParent = {(func)  => { this.setVolume = func }}
              />
            </div>

            <div className={styles.buttons}>
              <div onClick={() => { this.setVolume(84) }}>
                {`${volume == 84 ? '' : '*'}${volume}`}
              </div>
              <SoloButton isDisabled={true} />
              <MuteButton isDisabled={true} />
            </div>

          </div>

        </div>

      </div>
    )
  }
}

export default ClickVolume
