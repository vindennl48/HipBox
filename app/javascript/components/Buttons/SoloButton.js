import React from "react"
import PropTypes from "prop-types"

import ToggleButton from "./ToggleButton"

const SoloButton = (props) => (
  <ToggleButton
    {...props}
    className = {`cs-btn ${props.isDisabled ? 'disabled' : 'toggle solo'}`}
  >
    Solo
  </ToggleButton>
)
export default SoloButton
