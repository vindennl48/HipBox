import React from "react"
import PropTypes from "prop-types"
import styles from "./ChannelStripStyles"

import RecButton from "./RecButton"


class ExtraChanStrip extends React.Component {
  constructor(props) {
    super(props)
    this.state = { volume: 0 }
  }

  render () {
    const { volume } = this.state
    const { currentUser } = this.props
    return (
      <div className={styles.wrapper}>

        <div className={styles.border}>

          <div className={styles.information}>
            <div>Extra</div>
          </div>

          <div className={styles.body_extra}>
            <div className={styles.buttons}>
              <RecButton variable="record_toggle" />
            </div>
          </div>


        </div>

      </div>
    )
  }
}

export default ExtraChanStrip
