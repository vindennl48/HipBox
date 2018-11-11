class Api::V1::VariablesController < Api::V1::BaseController

  def index
    render json: Variable.find_by(name: params[:name])
  end

  def update
    variable = Variable.find(params[:id])
    variable.update_attributes(variable_params)
    #SaveVariableJob.perform_later variable_params
    send_to_daw(variable)
    render json: variable
  end

  def variable_params
    params.require(:variable).permit(:id, :name, :status, :value)
  end

  private
    def send_to_daw(variable)
      $OSCRUBY.send OSC::Message.new(get_osc(variable), parse_data(variable))
    end

    def get_osc(variable)
      return "/#{variable.name}"
    end

    def parse_data(variable)
      if variable.type_of == 'boolean'
        return (variable.status ? 1.0 : 0.0)
      else #elsif variable.type_of == 'value'
        return (variable.value.to_f / 127.0).round(3)
      end
    end

end
