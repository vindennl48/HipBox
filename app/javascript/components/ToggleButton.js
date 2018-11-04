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
    let   { label, color } = this.props
    const { value } = this.state
    if (color == 'red')    { color = `${styles.on} ${styles.red}` }
    if (color == 'yellow') { color = `${styles.on} ${styles.yellow}` }
    if (color == 'green')  { color = `${styles.on} ${styles.green}` }
    return (
      <div
        className={`${styles.wrapper} ${value ? color : styles.off}`}
        onClick={this.handleClick}
      >
        {label}
      </div>
    );
  }
}

export default ToggleButton
