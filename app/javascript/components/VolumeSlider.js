import React from "react"
import PropTypes from "prop-types"

import Slider from "./Slider"


class VolumeSlider extends React.Component {
  //constructor (props) {
    //super(props)

    //this.state = {
      //volume: null,
    //}
  //}

  //componentDidMount() {
    //const { variable } = this.props

    //if (variable) {
      //fetch("/api/v1/variables", {
        //method: 'PUT',
        //headers: {
          //'Content-Type': 'application/json'
        //},
        //body: JSON.stringify({ name: variable })
      //})
        //.then((response) => { return response.json() })
        //.then((volume)     => {
          //this.setValue(volume.value)
          //this.setState({ volume: {
            //id:    volume.id,
            //value: volume.value,
          //}})
        //})
    //}
  //}

  //save = (value) => {
    //let { volume } = this.state

    //if (volume) {
      //volume.value = value

      //this.setState({ volume: volume })

      //fetch(`/api/v1/variables/${volume.id}`, {
        //method: 'PUT',
        //headers: {
          //'Content-Type': 'application/json'
        //},
        //body: JSON.stringify(volume),
      //})
        //.then((response) => { return response.json() })
        ////.then((volume)     => { console.log(volume) })
    //}
  //}

  //setValue() { [> call-forward for Slider.setValue <] }

  //render () {
    //const { volume } = this.state
    //return (
      //<Slider
        //callback = {this.save}
        //setValue = {{func} => { this.setValue = func }}
      ///>
    //)
  //}
}

VolumeSlider.defaultProps = {
  variable: null,
}

export default VolumeSlider
