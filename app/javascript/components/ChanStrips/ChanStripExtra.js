import React from "react"
import PropTypes from "prop-types"

import RecButton from "../Buttons/RecButton"


class ChanStrip extends React.Component {

  static defaultProps = {
  }

  capitalize(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
  }

  render () {
    return (
      <div className="channel-strip">

        <div className="information">
          Extra
        </div>

        <div className="body-extra">

          <div className="buttons">
            <div className="wrapper">
              <RecButton />
            </div>
          </div>

        </div>

      </div>
    )
  }
}
export default ChanStrip
