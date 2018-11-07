import React from "react"
import PropTypes from "prop-types"
import styles from "./ChannelStripStyles"

import BassFxToggle from "./BassFxToggle"


class BassEffects extends React.Component {
  constructor(props) {
    super(props)
    this.state = { volume: 0 }
  }

  render () {
    const { volume } = this.state
    const { currentUser } = this.props

    return (
      <div className={styles.wrapper_extra_wide}>

        <div className={styles.border}>

          <div className={styles.information}>
            <div>FX</div>
          </div>

          <div className={styles.body_extra}>
            <div className={styles.fx_buttons}>
              <BassFxToggle
                label="Lo-Cut"
                variable="mitch_fx_locut"
                bgColorActive="#aeff00"
              />
              <BassFxToggle
                label="Hi-Cut"
                variable="mitch_fx_hicut"
                bgColorActive="#fff600"
              />
              <BassFxToggle
                label="Dist"
                variable="mitch_fx_dist"
                bgColorActive="#ff7700"
              />
              <BassFxToggle
                label="Delay"
                variable="mitch_fx_delay"
                bgColorActive="#00f2ff"
              />
              <BassFxToggle
                label="Reverb"
                variable="mitch_fx_reverb"
                bgColorActive="#ff00e1"
              />
            </div>
          </div>

        </div>

      </div>
    )
  }
}

export default BassEffects
