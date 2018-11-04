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
    let   { label, color }         = this.props
    let   { isDisabled, isRemote } = this.props
    const { value }                = this.state
    let classes                    = `${styles.wrapper}`

    if      (isDisabled)        { classes += ` ${styles.disabled}` }
    else if (color == 'red')    { classes += value ? ` ${styles.on} ${styles.red}`    : ` ${styles.off}` }
    else if (color == 'yellow') { classes += value ? ` ${styles.on} ${styles.yellow}` : ` ${styles.off}` }
    else if (color == 'green')  { classes += value ? ` ${styles.on} ${styles.green}`  : ` ${styles.off}` }
    if      (isRemote)          { classes += ` ${styles.remote}` }

    return (
      <div
        className={classes}
        onClick={isDisabled ? () => {} : this.handleClick}
      >
        {label}
      </div>
    );
  }
}

export default ToggleButton
