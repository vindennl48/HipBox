import React from "react"
import PropTypes from "prop-types"

import ChanStripButton from "./ChanStripButton"

const RecButton = (props) => (
  <ChanStripButton
    className = "cs-btn toggle rec"
    variable  = "record_toggle"
  >
    REC
  </ChanStripButton>
)
export default RecButton
