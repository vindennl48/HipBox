import React from "react"
import PropTypes from "prop-types"

import Slider from "react-rangeslider"


class Slider extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      value: 0,
    }
  }

  componentDidMount() {
    this.props.setValue((value) => {
      this.setState({ value: value })
    })
  }

  change = (value) => {
    this.setState({ value: value, })
    this.props.callback(value)
  }

  render () {
    const { value } = this.state
    return (
      <Slider
        value       = {value}
        min         = {0}
        max         = {127}
        tooltip     = {false}
        orientation = "vertical"
        onChange    = {this.change}
      />
    )
  }
}

Slider.defaultProps = {
  setValue: () => {},
  callback: () => {},
}

export default Slider
