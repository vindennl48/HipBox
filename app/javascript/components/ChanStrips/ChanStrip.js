import React from "react"
import PropTypes from "prop-types"

import MuteButton from "../Buttons/MuteButton"
import SoloButton from "../Buttons/SoloButton"


class ChanStrip extends React.Component {
  render () {
    return (
      <div className="channel-strip">

        <div className="information">
          Label
        </div>

        <div className="body">

          <div className="slider">
            hey
          </div>

          <div className="buttons">
            <SoloButton isDisabled={true} variable="mitch_mitch_solo" />
            <MuteButton />
          </div>

        </div>

      </div>
    )
  }
}
export default ChanStrip
