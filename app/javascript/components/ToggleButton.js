import React from "react"
import PropTypes from "prop-types"

import Button from "./ToggleButtonStyles"


class ToggleButton extends React.Component {

  constructor (props) {
    super(props)
    this.state = {
      value: false,
    }
  }

  componentDidMount() {
    this.props.setValue((value) => {
      this.setState({ value: value })
    })
  }

  click = () => {
    let { value } = this.state
    value = value ? false : true
    this.setState({ value: value, })
    this.props.callback(value)
  }

  render () {
    const { value } = this.state
    const { isDisabled, isRemote } = this.props
    return (
      <Button
        {...this.props}
        isRemote = {isDisabled && value ? true : isRemote }
        onClick  = {isDisabled ? () => {} : this.click}
        value    = {isDisabled ? false : value}
      >
        {this.props.label}
      </Button>
    );
  }
}

ToggleButton.defaultProps = {
  setValue:          () => {},
  callback:          () => {},
  label:             'no lbl',
  isDisabled:        false,
  isRemote:          false,
  bgColorActive:     '#50f442',
  textColorActive:   '#000',
  remoteColorActive: '#00aaaa',
  width:             '40px',
  height:            '40px',
  mediaQuery:        `
    @media only screen and (min-width: 521px) { width: 40px; }
    @media only screen and (max-width: 520px) { width: 9vw; }
  `,
}

export default ToggleButton
