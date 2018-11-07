import React from "react"
import PropTypes from "prop-types"
import styled from "styled-components"


let Button = styled.div`
  display:       grid;
  place-items:   center;
  @media only screen and (min-width: 521px) { width: 50px; }
  @media only screen and (max-width: 520px) { width: 18vw; }
  height:        30px;
  border-radius: 5px;
  font-size:     12px;
  border:        1px solid #777;
  box-shadow:    none;
  transition:    all 0.3s ease;

  ${props => props.isDisabled ? `
    color:  #444;
    border: 1px solid #444;
  ` : ''}

  ${props => props.isRemote ? `
    background-color: ${props.remoteColorActive};
  ` : ''}

  ${props => props.value ? `
    box-shadow:       inset 2px 2px 5px #222;
    color:            ${props.textColorActive};
    background-color: ${props.bgColorActive};
    transition:       all 0.3s ease;
  ` : `
    transition: all 0.3s ease;
  `}
`;

class ToggleButton extends React.Component {

  constructor (props) {
    super(props)
    this.state = {
      value: false,
    }
  }

  componentDidMount() {
    this.props.setValue((value) => {
      this.setState({ value: value })
    })
  }

  click = () => {
    let { value } = this.state
    value = value ? false : true
    this.setState({ value: value, })
    this.props.callback(value)
  }

  render () {
    const { value } = this.state
    const { isDisabled, isRemote } = this.props
    return (
      <Button
        {...this.props}
        isRemote = {isDisabled && value ? true : isRemote }
        onClick = {isDisabled ? () => {} : this.click}
        value   = {isDisabled ? false : value}
      >
        {this.props.label}
      </Button>
    );
  }
}

ToggleButton.defaultProps = {
  setValue:          () => {},
  callback:          () => {},
  label:             'no lbl',
  isDisabled:        false,
  isRemote:          false,
  bgColorActive:     '#50f442',
  textColorActive:   '#000',
  remoteColorActive: '#00aaaa',
}

export default ToggleButton
