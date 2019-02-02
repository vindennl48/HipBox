import React from "react"
import PropTypes from "prop-types"


class ToggleButton extends React.Component {
  static defaultProps = {
    value:      0,
    name:       "name",
    isDisabled: false,
    callback:   ()=>{},
  }

  render () {
    const { value, name, isDisabled, callback } = this.props
    let classExtra  = isDisabled ? 'disabled' : `toggle ${name}`
    let className   = `cs-btn ${classExtra}`

    return (
      <div
        className = {`${className} ${value ? 'true' : 'false'}`}
        onClick   = { isDisabled ? ()=>{} : ()=>{callback(value ? 0 : 1)} }
      >
        {name}
      </div>
    );
  }
}
export default ToggleButton
