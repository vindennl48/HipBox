import React from "react"
import PropTypes from "prop-types"
import WideToggleButton from "./WideToggleButton"


class BassFxToggle extends React.Component {
  constructor (props) {
    super(props)

    this.state = {
      fx: null,
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
            this.setValue(fx.status)
            this.setState({ fx: {
              id:     fx.id,
              status: fx.status,
            }})
          })
        setTimeout(pollServer, 1000)
      }
      setTimeout(pollServer, 1000)
    }
  }

  save = (value) => {
    let { fx } = this.state

    if (fx) {
      fx.status = value

      this.setState({ fx: fx })

      fetch(`/api/v1/variables/${fx.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(fx),
      })
        .then((response) => { return response.json() })
        //.then((fx)     => { console.log(fx) })
    }
  }

  setValue() { /* call-forward for ToggleButton.setValue */ }

  render () {
    return (
      <WideToggleButton
        {...this.props}
        callback={this.save}
        setValue={(func) => { this.setValue = func }}
      />
    );
  }
}

BassFxToggle.defaultProps = {
  variable: null,
}

export default BassFxToggle
