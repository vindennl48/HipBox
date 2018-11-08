import React from "react"
import PropTypes from "prop-types"

import ChanStripButton from "./ChanStripButton"

const SoloButton = (props) => (
  <ChanStripButton
    className = {`cs-btn ${props.variable ? 'toggle solo' : 'disabled' }`}
    variable  = {props.variable}
  >
    Solo
  </ChanStripButton>
)
export default SoloButton
