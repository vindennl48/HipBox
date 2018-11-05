import React from "react"
import PropTypes from "prop-types"
import styles from "./ChannelStripStyles"

import Slider from "react-rangeslider"
import ToggleButton from "./ToggleButton"

class ChannelStrip extends React.Component {
  constructor (props) {
    super(props)
    this.state = {
      value: 0,
    }
  }

  componentDidMount() {
    this.props.setValue(this.setValue)
    this.props.setMute(this.setMute)
  }

  setValue = (value) => {
    this.props.onChange(value)
    this.setState({ value: value })
  }

  resetValue = () => {
    this.setValue(84)
  }

  render () {
    const { value }                      = this.state
    const { label, soloIsDisabled }      = this.props
    const { muteIsDisabled, soloRemote } = this.props
    return (
      <div className={styles.wrapper}>

        <div className={styles.border}>

          <div className={styles.information}>
            <div>{label}</div>
          </div>

          <div className={styles.body}>

            <div className={styles.slider}>
              <Slider
                value       = {value}
                min         = {0}
                max         = {127}
                tooltip     = {false}
                orientation = "vertical"
                onChange    = {this.setValue}
              />
            </div>

            <div className={styles.buttons}>
              <div onClick={this.resetValue}>{`${value == 84 ? '' : '*'}${value}`}</div>
              <ToggleButton
                label      = "Solo"
                color      = "yellow"
                isDisabled = {soloIsDisabled}
                isRemote   = {soloRemote}
              />
              <ToggleButton
                label      = "Mute"
                color      = "red"
                isDisabled = {muteIsDisabled}
                onChange   = {this.props.onMuteChange}
                setValue   = {(func) => { this.setMute = func }}
              />
            </div>

          </div>

        </div>

      </div>
    );
  }
}

ChannelStrip.defaultProps = {
  setValue:       () => {},
  setMute:        () => {},
  onChange:       () => {},
  value:          0,
  label:          'no lbl',
  soloIsDisabled: false,
  muteIsDisabled: false,
  soloRemote:     false,
}


export default ChannelStrip
