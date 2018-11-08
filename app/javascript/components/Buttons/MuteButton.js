import React from "react"
import PropTypes from "prop-types"

import ChanStripButton from "./ChanStripButton"

const MuteButton = (props) => (
  <ChanStripButton
    className = "cs-btn toggle mute"
    variable  = {props.variable}
  >
    Mute
  </ChanStripButton>
)
export default MuteButton
