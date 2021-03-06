<?php

/**
 * Форма аминистративной установки модуля (параметры)
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function uc_buhsoft_settings_form($form, &$form_state)
{
    $response = _uc_buhsoft_http_get_request(BUHSOFT_API_URL, BUHSOFT_METHOD_SERVER_INFO, array('token' => variable_get('uc_buhsoft_token', '')));


    if (($response->code != 200) || (isset($response->error))) {
        $form['uc_buhsoft_status'] = array(
            '#type' => 'markup',
            '#markup' => t('Exception is "%exception%". Status code is "%code%".', array('%exception%' => $response->error, '%code%' => $response->code)),
        );
    } else {
        $data = json_decode($response->data);
        if ($data->ok == 0) {
            $form['uc_buhsoft_status'] = array(
                '#type' => 'markup',
                '#markup' => t('Server status is "%error_message%". Error code is "%error_code%".', array('%error_message%' => $data->error_message, '%error_code%' => $data->error_code)),
            );
        } elseif ($data->ok == '1') {
            $form['uc_buhsoft_status'] = array(
                '#type' => 'markup',
                '#prefix' => '</br>',
                '#markup' => t('Server status is ok.'),
            );
            $form['uc_buhsoft_status_0'] = array(
                '#type' => 'markup',
                '#prefix' => '</br>',
                '#markup' => t('ServerWorker is "%serverWorker%".', array('%serverWorker%' => $data->info->serverWorker)),
            );
            $form['uc_buhsoft_status_1'] = array(
                '#type' => 'markup',
                '#prefix' => '</br>',
                '#markup' => t('Period is "%period%".', array('%period%' => $data->info->period)),
            );
            $form['uc_buhsoft_status_2'] = array(
                '#type' => 'markup',
                '#prefix' => '</br>',
                '#markup' => t('Guid_cashier is "%guid_cashier%".', array('%guid_cashier%' => $data->info->guid_cashier)),
            );
            $form['uc_buhsoft_status_3'] = array(
                '#type' => 'markup',
                '#prefix' => '</br>',
                '#markup' => t('Guid_kkm is "%guid_kkm%".', array('%guid_kkm%' => $data->info->guid_kkm)),
            );
            $form['uc_buhsoft_status_4'] = array(
                '#type' => 'markup',
                '#prefix' => '</br>',
                '#markup' => t('LastDate_check is "%lastDate_check%".', array('%lastDate_check%' => $data->info->lastDate_check)),
            );

            if (isset($data->info->mustBePrinted)) {
                $form['uc_buhsoft_status_5'] = array(
                    '#type' => 'markup',
                    '#prefix' => '</br>',
                    '#markup' => t('MustBePrinted is "%mustBePrinted%".', array('%mustBePrinted%' => $data->info->mustBePrinted)),
                );
            }

        }
    }

    $form['uc_buhsoft_token'] = array(
        '#type' => 'textfield',
        '#title' => t('Token for Buhsoft'),
        '#default_value' => variable_get('uc_buhsoft_token', ''),
        '#description' => t('Set token for online checkout'),
        '#required' => true,
    );

    $form['uc_buhsoft_diller'] = array(
        '#type' => 'textfield',
        '#title' => t('INN of store'),
        '#default_value' => variable_get('uc_buhsoft_diller', ''),
        '#required' => true,
    );

    $form['uc_buhsoft_cms'] = array(
        '#type' => 'textfield',
        '#title' => t('CMS Name'),
        '#default_value' => variable_get('uc_buhsoft_cms', 'Drupal 7 / Ubercart 3'),
        '#required' => true,
    );

    $form['uc_buhsoft_default_phone'] = array(
        '#type' => 'textfield',
        '#title' => t('Default customer phone'),
        '#default_value' => variable_get('uc_buhsoft_default_phone', variable_get('uc_store_phone', '')),
        '#description' => t('Set default customer phone for online checkout'),
        '#required' => true,
    );

    $form['uc_buhsoft_default_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Default customer email'),
        '#default_value' => variable_get('uc_buhsoft_default_email', variable_get('uc_store_email', '')),
        '#description' => t('Set default customer Email for online checkout'),
        '#required' => true,
    );

    $form['uc_buhsoft_print'] = array(
        '#type' => 'select',
        '#title' => t('Print document'),
        '#options' => array(
            0 => t('No'),
            1 => t('Yes'),
        ),
        '#default_value' => variable_get('uc_buhsoft_print', 1),
    );

    $form['uc_buhsoft_name_cashier'] = array(
        '#type' => 'textfield',
        '#title' => t('Name of cashier'),
        '#default_value' => variable_get('uc_buhsoft_name_cashier', ''),
        '#required' => false,
    );

    $payment_methods = array_filter(_uc_payment_method_list(), function ($v, $k) {
        return $v['checkout'] == true;
    }, ARRAY_FILTER_USE_BOTH);

    $form['payment_methods'] = array(
        '#type' => 'fieldset',
        '#title' => t('Select payment methods for online checkout'),
        '#collapsible' => false,
        '#collapsed' => false,
        '#description' => t('If you want receive online checkout when got 100% payment with select methods'),
    );

    foreach ($payment_methods as $key => $payment_method) {
        $form['payment_methods']['uc_buhsoft_method_' . $key] = array(
            '#type' => 'checkbox',
            '#title' => $payment_method['title'],
            '#description' => $payment_method['desc'],
            '#default_value' => variable_get('uc_buhsoft_method_' . $key, false),
            '#return_value' => true,
        );
    }

    $product_types = uc_product_types();

    $form['vats'] = array(
        '#type' => 'fieldset',
        '#title' => t('Set VAT% for product types'),
        '#collapsible' => false,
        '#collapsed' => false,
        '#description' => t('Set products types VAT, in store each products type has self VAT rate'),
    );

    foreach ($product_types as $product_type) {
        $form['vats']['uc_buhsoft_vat_' . $product_type] = array(
            '#type' => 'select',
            '#title' => $product_type,
            '#options' => array(
                1 => t('18% VAT'),
                2 => t('10% VAT'),
                3 => t('0% VAT'),
                4 => t(' Without VAT'),
            ),
            '#default_value' => variable_get('uc_buhsoft_vat_' . $product_type, 1),
            '#required' => true,
        );
    }

    $form['vats']['uc_buhsoft_vat_delivery'] = array(
        '#type' => 'select',
        '#title' => t('Select VAT rate for delivery'),
        '#options' => array(
            1 => t('18% VAT'),
            2 => t('10% VAT'),
            3 => t('0% VAT'),
            4 => t(' Without VAT'),
        ),
        '#default_value' => variable_get('uc_buhsoft_vat_delivery', 1),
    );

    // Возвращаем форму с кнопкой
    return system_settings_form($form);

}