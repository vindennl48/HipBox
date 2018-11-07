import React from "react"
import PropTypes from "prop-types"

import RecButton from "./RecButton"
import HPRerouteButton from "./HPRerouteButton"


class ExtraChanStrip extends React.Component {
  constructor(props) {
    super(props)
    this.state = { volume: 0 }
  }

  render () {
    const { volume } = this.state
    const { currentUser } = this.props

    let hpr = null
    if (currentUser == 'mitch') {
      hpr = <HPRerouteButton variable="hpr_toggle" />
    }

    return (
      <div className="chan-strip wrapper">

        <div className="border">

          <div className="information">
            <div>Extra</div>
          </div>

          <div className="body_extra">
            <div className="buttons">
              {hpr}
              <RecButton variable="record_toggle" />
            </div>
          </div>

        </div>

      </div>
    )
  }
}

export default ExtraChanStrip
