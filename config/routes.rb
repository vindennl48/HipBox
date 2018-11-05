Rails.application.routes.draw do
  namespace :api do
    namespace :v1 do
      resources :users, only: [:index, :update]
      resources :variables, only: [:update]
      put 'variables', to: 'variables#index'
    end
  end

  devise_for :users
  get 'home/index'
  root 'home#index'

end
