import React from "react"
import PropTypes from "prop-types"

import MuteButton from "../Buttons/MuteButton"
import SoloButton from "../Buttons/SoloButton"
import ValueDisplay from "../Displays/ValueDisplay"
import ChanStripSlider from "../Sliders/ChanStripSlider"


class ChanStrip extends React.Component {

  static defaultProps = {
  }

  capitalize(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
  }

  render () {

    const { user, chan, label } = this.props

    return (
      <div className="channel-strip">

        <div className="information">
          {label}
        </div>

        <div className="body">

          <div className="slider">
            <ChanStripSlider
              variable={`${user}_${chan}_vol`}
              setValue={(func)=>{ this.setVolume = func }}
              callback={(value)=>{ this.setDisplay(value) }}
            />
          </div>

          <div className="buttons">
            <div className="wrapper">
              <ValueDisplay
                setValue={(func)=>{ this.setDisplay = func }}
                callback={(value)=>{ this.setVolume(value) }}
              />
              <SoloButton />
              <MuteButton
                variable={chan == 'talkback' ? 'talkback_toggle' : ''}
              />
            </div>
          </div>

        </div>

      </div>
    )
  }
}
export default ChanStrip
