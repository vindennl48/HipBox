import React from "react"
import PropTypes from "prop-types"

import MuteButton from "../Buttons/MuteButton"
import SoloButton from "../Buttons/SoloButton"
import ValueDisplay from "../Displays/ValueDisplay"
import ChanStripSlider from "../Sliders/ChanStripSlider"


class ChanStrip extends React.Component {
  render () {
    return (
      <div className="channel-strip">

        <div className="information">
          Label
        </div>

        <div className="body">

          <div className="slider">
            <ChanStripSlider
              variable="mitch_mitch_vol"
              setValue={(func)=>{ this.setVolume = func }}
              callback={(value)=>{ this.setDisplay(value) }}
            />
          </div>

          <div className="buttons">
            <ValueDisplay
              setValue={(func)=>{ this.setDisplay = func }}
              callback={(value)=>{ this.setVolume(value) }}
            />
            <SoloButton isDisabled={true} variable="mitch_mitch_solo" />
            <MuteButton />
          </div>

        </div>

      </div>
    )
  }
}
export default ChanStrip
