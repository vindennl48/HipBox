import React from "react"
import PropTypes from "prop-types"

import ReactSlider from "react-rangeslider"
import ToggleButton from "./ToggleButton"


class ChanStrip extends React.Component {

  static defaultProps = {
  }

  capitalize(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
  }

  render () {

    const { user, chan, volu, mute, solo, callback } = this.props

    return (
      <div className="channel-strip">

        <div className="information">
          {this.capitalize(chan)}
        </div>

        <div className="body">

          <div className="slider">
            <ReactSlider
              value       = {volu["value"]}
              min         = {0}
              max         = {127}
              tooltip     = {false}
              orientation = "vertical"
              onChange    = {(value) => {
                callback(chan, "vol", value)
              }}
            />
          </div>

          <div className="buttons">
            <div className="wrapper">

              <div className="cs-value-display">
                <div onClick={ ()=>{callback(chan, "vol", 84)} }>
                  {`${volu["value"] !=84 ? '*' : ''}${volu["value"]}`}
                </div>
              </div>

              <ToggleButton
                name       = "mute"
                value      = {mute != null ? mute["value"] : 0}
                isDisabled = {
                  user == chan ||
                  mute == null
                    ? true : false
                }
                callback   = { (value)=>{callback(chan, "mute", value)} }
              />

              <ToggleButton
                name       = "solo"
                value      = {solo != null ? solo["value"] : 0}
                isDisabled = {
                  user != chan ||
                  solo == null
                    ? true : false
                }
                callback   = { (value)=>{callback(chan, "solo", value)} }
              />
            </div>
          </div>

        </div>

      </div>
    )
  }
}
export default ChanStrip
