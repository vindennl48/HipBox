import React from "react"
import PropTypes from "prop-types"

import ChanStripButton from "./ChanStripButton"

const MuteButton = (props) => (
  <ChanStripButton
    isDisabled = {props.isDisabled}
    className  = {`cs-btn ${props.variable ? 'toggle mute' : 'disabled' }`}
    variable   = {props.variable}
  >
    Mute
  </ChanStripButton>
)
export default MuteButton
