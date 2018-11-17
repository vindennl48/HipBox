class Api::V1::VariablesController < Api::V1::BaseController
  def index
    render json: Variable.find_by(name: params[:name])
  end
end
