import React from "react"
import PropTypes from "prop-types"
import styles from "./ChannelStripStyles"

import RecButton from "./RecButton"
import HPRerouteButton from "./HPRerouteButton"


class ExtraChanStrip extends React.Component {
  constructor(props) {
    super(props)
    this.state = { volume: 0 }
  }

  render () {
    const { volume } = this.state
    const { currentUser } = this.props

    let hpr = null
    if (currentUser == 'mitch') {
      hpr = <HPRerouteButton variable="hpr_toggle" />
    }

    return (
      <div className={styles.wrapper}>

        <div className={styles.border}>

          <div className={styles.information}>
            <div>Extra</div>
          </div>

          <div className={styles.body_extra}>
            <div className={styles.buttons}>
              {hpr}
              <RecButton variable="record_toggle" />
            </div>
          </div>

        </div>

      </div>
    )
  }
}

export default ExtraChanStrip
