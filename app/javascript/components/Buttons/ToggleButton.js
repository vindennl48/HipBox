import React from "react"
import PropTypes from "prop-types"


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
    const { value }      = this.state
    return (
      <div
        className = {`${this.props.className} ${value}`}
        onClick   = {this.click}
      >
        {this.props.children}
      </div>
    );
  }
}

ToggleButton.defaultProps = {
  setValue:   () => {},
  callback:   () => {},
}

export default ToggleButton
