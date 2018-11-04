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

  handleChange = (value) => {
    this.setState({
      value: value
    })
  }

  render () {
    const { value } = this.state
    return (
      <div className={styles.wrapper}>

        <div className={styles.border}>

          <div className={styles.information}>
            <div>James</div>
          </div>

          <div className={styles.body}>

            <div className={styles.slider}>
              <Slider
                value={value}
                min={0}
                max={127}
                tooltip={false}
                orientation="vertical"
                onChange={this.handleChange}
              />
            </div>

            <div className={styles.buttons}>
              <ToggleButton label="Solo" />
              <ToggleButton label="Mute" />
            </div>

          </div>

        </div>

      </div>
    );
  }
}

export default ChannelStrip
