import React from "react"
import PropTypes from "prop-types"

import ChanStripButton from "./ChanStripButton"

const SoloButton = (props) => (
  <ChanStripButton
    isDisabled = {props.isDisabled}
    className  = {`cs-btn ${props.variable && !props.isDisabled ? 'toggle solo' : 'disabled' }`}
    variable   = {props.variable}
  >
    Solo
  </ChanStripButton>
)
export default SoloButton
