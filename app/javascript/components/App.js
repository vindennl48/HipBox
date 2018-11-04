import React from "react"
import PropTypes from "prop-types"
import styles from "./AppStyles"

import ChannelStrip from "./ChannelStrip"


class App extends React.Component {
  render () {
    return (
      <div className={styles.wrapper}>
        <ChannelStrip label="James" />
        <ChannelStrip label="Jesse" />
        <ChannelStrip label="Mitch" />
        <ChannelStrip label="Drums" />
        <ChannelStrip label="HP Vol" />
      </div>
    )
  }
}

export default App
