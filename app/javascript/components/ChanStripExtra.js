import React from "react"
import PropTypes from "prop-types"

import ToggleButton from "./ToggleButton"


class ChanStrip extends React.Component {

  static defaultProps = {
  }

  capitalize(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
  }

  render () {
    const { chan, rec, callback } = this.props

    return (
      <div className="channel-strip">

        <div className="information">
          Extra
        </div>

        <div className="body-extra">

          <div className="buttons">
            <div className="wrapper">
              <ToggleButton
                name       = "REC"
                value      = {rec != null ? rec["value"] : 0}
                isDisabled = {rec == null ? true : false}
                callback   = { (value)=>{callback(chan, "record", 1)} }
              />
            </div>
          </div>

        </div>

      </div>
    )
  }
}
export default ChanStrip
