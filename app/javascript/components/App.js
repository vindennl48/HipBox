import React from "react"
import PropTypes from "prop-types"
import VolumeSlider from "./VolumeSlider"
import styles from "./AppStyles"


class App extends React.Component {
  render () {
    return (
      <React.Fragment>
        <div className={styles.wrapper}>
          <VolumeSlider label="James" />
          <VolumeSlider label="Jesse" />
          <VolumeSlider label="Mitch" />
          <VolumeSlider label="Drums" />
          <VolumeSlider label="HP Vol" />
          <VolumeSlider label="Click" />
          <VolumeSlider label="TlkBk" />
        </div>
      </React.Fragment>
    );
  }
}

export default App
