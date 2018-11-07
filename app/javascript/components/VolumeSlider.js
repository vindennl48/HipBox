import React from "react"
import PropTypes from "prop-types"

import SlideBar from "./SlideBar"


class VolumeSlider extends React.Component {
  constructor (props) {
    super(props)
    this.state = { volume: null }
  }

  componentDidMount() {
    const { variable, getValueFromParent } = this.props

    // send function to get values from parent
    getValueFromParent((value) => {
      this.setValue(value)
      this.save(value)
      this.sendValueToParent(value)
    })

    if (variable) {
      // pull initial data from server
      fetch("/api/v1/variables", {
        method:  'PUT',
        headers: { 'Content-Type':'application/json' },
        body:    JSON.stringify({ name: variable })
      })
        .then((response) => { return response.json() })
        .then((volume)   => {
          this.setValue(volume.value)
          this.sendValueToParent(volume.value)
          this.setState({ volume: {
            id:    volume.id,
            value: volume.value,
          }})
        })

      // send data to server every 1s
      let pollServer = () => {
        const { volume } = this.state
        fetch(`/api/v1/variables/${volume.id}`, {
          method: 'PUT',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify(volume),
        })
          .then((response) => { return response.json() })
          //.then((volume)     => { console.log(volume) })
        setTimeout(pollServer, 1000)
      }
      setTimeout(pollServer, 1000)
    }
  }

  // if parent wants upated values, send value
  sendValueToParent(value) {
    if (this.props.sendValueToParent) {
      this.props.sendValueToParent(value)
    }
  }

  save = (value) => {
    let { volume } = this.state

    this.sendValueToParent(value)

    //console.log('value: ', value)

    if (volume) {
      volume.value = value

      this.setState({ volume: volume })

      fetch(`/api/v1/variables/${volume.id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(volume),
      })
        .then((response) => { return response.json() })
        //.then((volume)     => { console.log(volume) })
    }
  }

  setValue() { /* call-forward for SlideBar.setValue */ }

  render () {
    const { volume } = this.state
    return (
      <SlideBar
        callback = {this.save}
        setValue = {(func) => { this.setValue = func }}
      />
    )
  }
}

VolumeSlider.defaultProps = {
  variable: null,
}

export default VolumeSlider
