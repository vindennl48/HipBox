import React from "react"
import PropTypes from "prop-types"

import ChanStripButton from "./ChanStripButton"

const MuteButton = (props) => (
  <ChanStripButton
    className = {`cs-btn ${props.variable ? 'toggle mute' : 'disabled' }`}
    variable  = {props.variable}
  >
    Mute
  </ChanStripButton>
)
export default MuteButton
