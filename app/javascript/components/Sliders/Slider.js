import React from "react"
import PropTypes from "prop-types"

import ReactSlider from "react-rangeslider"


class Slider extends React.Component {
  static defaultProps = {
    setValue: ()=>{},
    callback: ()=>{},
  }

  constructor (props) {
    super(props)

    this.state = {
      value: 0,
    }
  }

  componentDidMount() {
    this.props.setValue((value) => {
      //this.setState({ value: value })
      this.change(value)
    })
  }

  change = (value) => {
    //this.setState({ value: value, })
    this.props.callback(value)
  }

  render () {
    //const { value } = this.state
    const { value } = this.props

    return (
      <ReactSlider
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

export default Slider
