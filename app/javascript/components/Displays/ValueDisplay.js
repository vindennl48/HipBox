import React from "react"
import PropTypes from "prop-types"


class ValueDisplay extends React.Component {

  static defaultProps = {
    setValue:   () => {},
    callback:   () => {},
  }

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

  click = () => {
    this.setState({ value: 84, })
    this.props.callback(84)
  }

  render () {
    const { value } = this.state
    return (
      <div className="cs-value-display">
        <div onClick={this.click}>
          {`${value !=84 ? '*' : ''}${value}`}
        </div>
      </div>
    );
  }
}
export default ValueDisplay
