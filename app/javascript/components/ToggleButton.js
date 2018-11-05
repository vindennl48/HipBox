import React from "react"
import PropTypes from "prop-types"
import styles from "./ToggleButtonStyles"


class ToggleButton extends React.Component {

  constructor (props) {
    super(props)
    this.state = {
      value: false,
    }
  }

  componentDidMount() {
    this.props.setValue(this.setValue)
  }

  setValue = (value) => {
    this.props.onChange(value)
    this.setState({ value: value })
  }

  handleClick = () => {
    let { value } = this.state

    //console.log('clicked')

    value = value ? false : true

    this.props.onChange(value)

    this.setState({
      value: value,
    })
  }

  render () {
    const { label, color }         = this.props
    const { isDisabled, isRemote } = this.props
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

ToggleButton.defaultProps = {
  setValue:   () => {},
  onChange:   () => {},
  label:      'no lbl',
  isDisabled: false,
  isRemote:   false,
}

export default ToggleButton
