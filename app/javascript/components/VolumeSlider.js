import React from "react"
import PropTypes from "prop-types"
import Slider from 'react-rangeslider'
import ToggleButton from "./ToggleButton"
import styles from "./VolumeSliderStyles"


class VolumeSlider extends React.Component {
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
    const { label } = this.props
    return (
      <div className={styles.wrapper}>
        <label>{label}</label>
        <label>{value}</label>
        <div className={styles.channel}>
          <Slider
            value={value}
            min={0}
            max={127}
            tooltip={false}
            orientation="vertical"
            onChange={this.handleChange}
          />
          <div className={styles.buttons}>
            <ToggleButton label="Solo" />
            <ToggleButton label="Mute" />
          </div>
        </div>
      </div>
    );
  }
}

export default VolumeSlider
