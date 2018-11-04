import React from "react"
import PropTypes from "prop-types"
import styles from "./ToggleButtonStyles"


class ToggleButton extends React.Component {

  constructor (props) {
    super(props)
    this.state = {
      value: false
    }
  }

  handleClick = () => {
    console.log('clicked')
    const { value } = this.state
    this.setState({
      value: value ? false : true,
    })
  }

  render () {
    const { label } = this.props
    const { value } = this.state
    return (
      <div
        className={`${styles.wrapper} ${value ? styles.on : styles.off}`}
        onClick={this.handleClick}
      >
        {label}
      </div>
    );
  }
}

export default ToggleButton
