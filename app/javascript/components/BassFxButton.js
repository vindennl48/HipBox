import React from "react"
import PropTypes from "prop-types"
import ToggleButton from "./ToggleButton"


class BassFxToggle extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      fx:        null,
      pollBlock: false,
    }
  }

  componentDidMount() {
    const { variable } = this.props

    if (variable) {
      let pollServer = () => {
        fetch("/api/v1/variables", {
          method: 'PUT',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({ name: variable })
        })
          .then((response) => { return response.json() })
          .then((fx)     => {
            if (!this.state.pollBlock) {
              this.setValue(fx.status)
              this.setState({ fx: {
                id:     fx.id,
                status: fx.status,
              }})
            } else {
              this.setState({ pollBlock: false })
            }
          })
          .catch(() => { location.reload() })
        setTimeout(pollServer, 1000)
      }
      pollServer()
    }
  }

  save = (value) => {
    let { fx } = this.state

    if (fx) {
      fx.status = value

      this.setState({
        fx: fx,
        pollBlock: true,
      })

      fetch(`/api/v1/variables/${fx.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(fx),
      })
        .then((response) => { return response.json() })
        //.then((fx) => { console.log(fx) })
    }
  }

  setValue() { /* call-forward for ToggleButton.setValue */ }

  render () {
    return (
      <ToggleButton
        {...this.props}
        callback={this.save}
        setValue={(func) => { this.setValue = func }}
        mediaQuery={`
          @media only screen and (min-width: 741px) { width: 50px; }
          @media only screen and (max-width: 740px) { width: 16vw; }
        `}
      />
    );
  }
}

BassFxToggle.defaultProps = {
  variable: null,
}

export default BassFxToggle
